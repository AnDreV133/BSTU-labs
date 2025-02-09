package org

import java.sql.*
import java.util.*

fun main() {
    mainProcess(connectionMode = ConnectionMode.CREATE)
}

fun connect(): Result<Connection> =
    try {
        Class.forName("org.postgresql.Driver")
        Result.success(
            DriverManager.getConnection(
                ConnectionData.URL,
                ConnectionData.USER,
                ConnectionData.PASSWORD
            ).apply { autoCommit = false }
        )
    } catch (e: SQLException) {
        Result.failure(e)
    }

sealed class State {
    class Connection : State()
    class ChangeStore(val conn: java.sql.Connection) : State()
    class Menu(val conn: java.sql.Connection, val storeId: Long) : State()
    class Shopping(val conn: java.sql.Connection, val storeId: Long) : State()
    class Exit(val conn: java.sql.Connection? = null) : State()
}

enum class ConnectionMode {
    CREATE,
    RECREATE,
    USER_INPUT
}

fun mainProcess(
    connectionMode: ConnectionMode = ConnectionMode.RECREATE,
    scanner: Scanner = Scanner(System.`in`),
) {
    var state: State = State.Connection()
    while (true) {
        when (state) {
            is State.Connection -> {
                connect().fold(
                    onFailure = { e ->
                        println(e)
                        println("Не удалось выполнить подключение.\nВведите 't', чтобы попробовать снова; 'e' чтобы выйти.")
                        while (true) {
                            val input = scanner.next().lowercase()
                            if (input == "e") {
                                state = State.Exit()
                                break
                            } else if (input == "t") {
                                break
                            } else {
                                println("Неизвестный символ, попробуйте снова")
                            }
                        }
                    },
                    onSuccess = { conn ->
                        val connection: Connection
                        when (connectionMode) {
                            ConnectionMode.CREATE -> {
                                connection = conn.executeInitDb().executePrepareDb()
                            }

                            ConnectionMode.RECREATE -> {
                                connection = conn.executeDeleteDb().executeInitDb().executePrepareDb()
                            }

                            ConnectionMode.USER_INPUT -> {
                                println("Вы желаете продолжить с введёнными данными? (y/n)")
                                while (true) {
                                    val input = scanner.next().lowercase()
                                    if (input == "y") {
                                        connection = conn.executeInitDb().executePrepareDb()
                                        break
                                    } else if (input == "n") {
                                        connection = conn.executeDeleteDb().executeInitDb().executePrepareDb()
                                        break
                                    } else {
                                        println("Неизвестный символ, попробуйте снова")
                                    }
                                }
                            }
                        }
                        state = State.ChangeStore(connection)
                    }
                )
            }

            is State.ChangeStore -> {
                val possibleIdList = mutableListOf<Long>()
                val castedState = (state as State.ChangeStore)
                castedState.conn.execute("select * from store;") {
                    while (it.next()) {
                        possibleIdList.add(it.getLong(1))
                        println(
                            StringJoiner("\t")
                                .add("id:" + it.getString(1))
                                .add("адрес:" + it.getString(2))
                        )
                    }
                    println("Введите id магазина:")
                    val storeId = scanner.next().toLong()
                    if (storeId in possibleIdList)
                        state = State.Menu(castedState.conn, storeId)
                    else {
                        println("Магазин с таким id не найден. Попробуйте снова:")
                        state = State.ChangeStore(castedState.conn)
                    }
                }
            }

            is State.Menu -> {
                val castedState = (state as State.Menu)
                println(
                    """
                    Выберите действие:
                    1. Сделать покупку
                    2. История покупок
                    3. Рейтинг товаров по магазину
                    4. Выход
                    """.trimIndent()
                )

                val input = scanner.next().toInt()
                when (input) {
                    1 -> state = State.Shopping(castedState.conn, castedState.storeId)
                    2 -> {
                        castedState.conn.execute(
                            """
                            select  t0.${Table.CheckList.ID},
                                    t2.${Table.Product.NAME}, 
                                    t1.${Table.Purchase.AMOUNT},
                                    t2.${Table.Product.QUANTITY_TO_ASSESS},
                                    t1.${Table.Purchase.AMOUNT}*t3.${Table.Accounting.COST}
                            from ${Table.CheckList.T_NAME} as t0 
                            inner join ${Table.Purchase.T_NAME} as t1
                            on t0.${Table.CheckList.ID}=t1.${Table.Purchase.CHECK_LIST_ID}
                                and t0.${Table.CheckList.STORE_ID}=${castedState.storeId}
                            inner join ${Table.Product.T_NAME} as t2
                            on t1.${Table.Purchase.T_NAME}=t2.${Table.Product.ARTICLE}   
                            inner join ${Table.Accounting.T_NAME} as t3
                            on t1.${Table.Purchase.PRODUCT_ARTICLE}=t3.${Table.Accounting.PRODUCT_ARTICLE}
                                and t3.${Table.Accounting.STORE_ID}=${castedState.storeId}
                            order by t0.${Table.CheckList.ID} asc;
                            """
                        ) {
                            var prevId: Long = -1
                            while (it.next()) {
                                val currId = it.getLong(1)
                                if (prevId != currId) {
                                    println("check id: $currId")
                                    prevId = currId
                                }
                                println(
                                    StringJoiner("\t")
                                        .add(it.getString(2))
                                        .add(it.getString(3) + "_" + it.getString(4))
                                        .add("цена: " + it.getString(5) + "р")
                                )
                            }
                        }
                    }

                    3 -> castedState.conn.execute(
                        """
                        select  t2.${Table.Product.NAME}, 
                                round(coalesce(sum(t1.${Table.Purchase.AMOUNT})*100/(select sum(amount) from ${Table.Purchase.T_NAME}),0),2) 
                                    as amount_in_percent 
                            from check_list as t0 
                            inner join ${Table.Purchase.T_NAME} as t1
                            on t0.id=t1.${Table.Purchase.CHECK_LIST_ID} 
                                and t0.${Table.CheckList.STORE_ID}=${castedState.storeId}
                            right join ${Table.Product.T_NAME} as t2
                            on t1.${Table.Purchase.PRODUCT_ARTICLE}=t2.${Table.Product.ARTICLE}
                            group by t2.${Table.Product.NAME}
                            order by amount_in_percent desc;
                        """
                    ) {
                        while (it.next())
                            println(
                                StringJoiner("\t")
                                    .add(it.getString(1))
                                    .add(it.getString(2)+"%")
                            )
                    }

                    4 -> {
                        println("До встречи!")
                        state = State.Exit()
                    }

                    else -> {
                        println("Неизвестный символ, попробуйте снова")
                        // -> State.Menu
                    }
                }
            }

            is State.Shopping -> {
                val castedState = state as State.Shopping

                println("Ассортимент:")
                val assortment = mutableListOf<Purchase>()
                castedState.conn.execute(
                    """
                        select  t0.${Table.Accounting.PRODUCT_ARTICLE},
                                t1.${Table.Product.NAME},
                                t0.${Table.Accounting.AMOUNT}, 
                                t0.${Table.Accounting.COST},
                                t1.${Table.Product.QUANTITY_TO_ASSESS}
                            from ${Table.Accounting.T_NAME} as t0
                            inner join ${Table.Product.T_NAME} as t1 
                            on t0.${Table.Accounting.PRODUCT_ARTICLE}=t1.${Table.Product.ARTICLE}
                            where ${Table.Accounting.STORE_ID}=${castedState.storeId};
                        """
                ) {
                    while (it.next()) {
                        assortment.add(Purchase(it.getLong(1), it.getDouble(3)))
                        println(
                            StringJoiner("\t")
                                .add(it.getLong(1).toString())
                                .add(it.getString(2))
                                .add(it.getDouble(3).toString())
                                .add(it.getDouble(4).toString() + "р")
                                .add(it.getString(5))
                        )
                    }
                }
                println("Выберите товары для покупки, артикул и количество через пробел.\nЧтобы завершить нажмите 'e'")
                val purchases = mutableListOf<Purchase>()
                while (true) {
                    val splitInput = scanner.nextLine().split(" ")
                    if (splitInput.size == 1 && splitInput[0] == "e") {
                        castedState.conn.executeBuy(castedState.storeId, purchases)
                        println("Покупка совершена успешно")
                        state = State.Menu(castedState.conn, castedState.storeId)
                        break
                    } else if (splitInput.size == 2) {
                        try {
                            val article = splitInput[0].toLong()
                            val amount = splitInput[1].toDouble()

                            val productFromAssortment = assortment.findLast { it.article == article }
                            if (productFromAssortment == null) {
                                println("Товар с таким артикулом не найден")
                            } else if (amount > productFromAssortment.amount) {
                                println("Недостаточно товара на складе")
                            } else {
                                purchases.add(Purchase(article, amount))
                                println("Товар с артиклом $article добавлен")
                            }
                        } catch (e: NumberFormatException) {
                            println("Некорректный ввод чисел")
                        }
                    } else if (splitInput.isEmpty() || splitInput[0]=="") {
                        // ignore
                    } else {
                        println("Некорректный ввод")
                    }
                }
            }

            is State.Exit -> {
                (state as State.Exit).conn?.close()
                break
            }
        }
    }
}

//fun test_init() {
//    connect()
//        .onFailure { e -> println("Connection failed: ${e.message}") }
//        .getOrNull()?.use { conn ->
//            conn
//                .executeDeleteDb()
//
//                .executeInitDb()
//
//                .executePrepareDb()
//
//                .execute("select * from store;") {
//                    while (it.next())
//                        println(
//                            StringJoiner("\t")
//                                .add("id:" + it.getString(1))
//                                .add("address:" + it.getString(2))
//                        )
//                }
//        }
//}