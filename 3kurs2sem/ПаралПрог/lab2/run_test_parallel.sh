gcc -fopenmp ./test.c -lm -o t.o 

if [ ! -f ./t.o ]
then
    echo compile error
    exit 1
fi

# Функция для преобразования времени в секунды
convert_to_seconds() {
    local time_str=$1
    # Убираем 'm' и 's', затем разбиваем на минуты и секунды
    local minutes=$(echo "$time_str" | sed 's/m.*//g')
    local seconds=$(echo "$time_str" | sed 's/.*m//g; s/s//g')
    # Вычисляем общее время в секундах
    echo "$minutes * 60 + $seconds" | bc
}

# Функция для выполнения эксперимента
run_experiment() {
    local input_arg=$1
    local key=$2
    echo "Эксперимент с ключом $key, кол-вом итераций: $input_arg"

    # Замер времени выполнения команды sleep с переданным аргументом
    output=$( { time ./t.o $input_arg $key; } 2>&1 )

    # Извлечение real, user и sys времени из вывода time
    real_time=$(echo "$output" | grep real | awk '{print $2}')
    user_time=$(echo "$output" | grep user | awk '{print $2}')
    sys_time=$(echo "$output" | grep sys | awk '{print $2}')

    # Преобразование времени в секунды
    real_seconds=$(convert_to_seconds "$real_time")
    user_seconds=$(convert_to_seconds "$user_time")
    sys_seconds=$(convert_to_seconds "$sys_time")

    # Вывод результатов в консоль
    echo "real_time: $real_seconds секунд"
    echo "user_time: $user_seconds секунд"
    echo "sys_time: $sys_seconds секунд"
    echo "-------------------------"

    # Запись результатов в CSV-файл
    echo "$input_arg;$real_seconds;$user_seconds;$sys_seconds" >> "$output_csv"
}

# Выполнение экспериментов
output_csv="timetest_static.csv"
echo "sep=;" > "$output_csv"
echo "input_arg;real_time;user_time;sys_time" >> "$output_csv"
for (( i=$1; i<=$2; i+=$3 ))
do
    run_experiment $i 's'
done

output_csv="timetest_dynamic.csv"
echo "sep=;" > "$output_csv"
echo "input_arg;real_time;user_time;sys_time" >> "$output_csv"
for (( i=$1; i<=$2; i+=$3 ))
do
    run_experiment $i 'd'
done

output_csv="timetest_guided.csv"
echo "sep=;" > "$output_csv"
echo "input_arg;real_time;user_time;sys_time" >> "$output_csv"
for (( i=$1; i<=$2; i+=$3 ))
do
    run_experiment $i 'g'
done

output_csv="timetest_auto.csv"
echo "sep=;" > "$output_csv"
echo "input_arg;real_time;user_time;sys_time" >> "$output_csv"
for (( i=$1; i<=$2; i+=$3 ))
do
    run_experiment $i 'a'
done

rm t.o
