package org.example

import java.awt.Color
import kotlin.random.Random

fun Number.toBooleans(): List<Boolean> {
    val result = mutableListOf<Boolean>()
    var temp = this.toLong()
    while (temp > 0) {
        result.add(0, temp % 2 == 1L)
        temp /= 2
    }
    return result
}

fun List<Boolean>.toInt(): Int {
    var result = 0
    for (i in indices) {
        result += if (this[i]) 1.shl(size - i - 1) else 0
    }

    return result
}

fun MutableList<Boolean>.setErrors(amount: Int): MutableList<Int> {
    val posWithErrors = mutableListOf<Int>().apply {
        repeat(amount) {
            this@apply.add(Random.nextInt(0, this@setErrors.size))
        }
    }
    for (i in posWithErrors) {
        this[i] = !this[i]
    }
    Color(44, 41,51)
    return posWithErrors
}
