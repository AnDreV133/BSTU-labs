import re

def cpp_to_uml(cpp_code):
    """Конвертирует код C++ в поля для записи в диаграмму UML.

    Args:
      cpp_code: Строка кода C++.

    Returns:
      Список полей UML.
    """

    # Извлечь имя класса
    class_name = re.findall(r"class (b*)", cpp_code)# .group(1)

    # Извлечь поля класса
    fields = re.findall(r"(w*?)s+(w*?);", cpp_code)

    # Извлечь методы класса
    methods = re.findall(r"(w*?)s+(w*?)((.*));", cpp_code)

    # Создать список полей UML
    uml_fields = []
    for field in fields:
        field_type, field_name = field
        uml_fields.append(f"- {field_name}: {field_type}")

    return class_name, methods, uml_fields


print(cpp_to_uml(
    """class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    GameWindow(GameMode *gameMode, QWidget *parent = nullptr);
    ~GameWindow();
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::GameWindow *ui;
    QTimer timerOne;
    QTimer timerTwo;
    GameMode *gameMode;
    QTime timePlayerOne;
    QTime timePlayerTwo;

    QString timerToString(QTime &time);

public slots:
    void startTimerOne();
    void startTimerTwo();
    void decrementTimeOne();
    void decrementTimeTwo();
    void switchTimer();
    void postNewLog(QString event);
};"""
))


print(cpp_to_uml(
    """class GameModesMenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameModesMenuWindow(QWidget *parent = nullptr);
    ~GameModesMenuWindow();
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnPVsC_clicked();

    void on_btnPVsPOfflane_clicked();

    void on_btnPVsPOnline_clicked();

private:
    Ui::GameModesMenuWindow *ui;
};"""
))




import re

class_map = {
    'public': 'public',
    'protected': 'protected',
    'private': 'private',
}


def parse_cpp_code(code):
    """Parses C++ code and converts it into fields for a UML diagram.

    Args:
        code (str): The C++ code to parse.

    Returns:
        list: A list of fields for the UML diagram.
    """

    fields = []

    # Iterate over the lines of code
    for line in code.split('n'):
        line = line.strip()

        # Skip empty lines
        if not line:
            continue

        # Handle class declarations
        if line.startswith('class'):
            class_name = line.split(' ')[1].split(':')[0]
            fields.append({
                'name': class_name,
                'type': 'Class',
                'visibility': 'public',
            })

        # Handle function declarations
        elif line.startswith('void'):
            function_name = line.split(' ')[1].split('(')[0]
            fields.append({
                'name': function_name,
                'type': 'Function',
                'visibility': 'public',
            })

        # Handle variable declarations
        elif line.endswith(';'):
            variable_name = line.split(' ')[1].split('=')[0]
            variable_type = line.split(' ')[0]
            fields.append({
                'name': variable_name,
                'type': variable_type,
                'visibility': None,
            })

    # Return the list of fields
    return fields


def convert_to_uml(fields):
    """Converts a list of fields into a UML diagram.

    Args:
        fields (list): The list of fields to convert.

    Returns:
        str: The UML diagram.
    """

    uml = ''

    # Iterate over the fields
    for field in fields:
        uml += f'{field["name"]}: {field["type"]}n'
        if field["visibility"]:
            uml += f'    {field["visibility"]}n'

    # Return the UML diagram
    return uml


print(convert_to_uml(parse_cpp_code(
    """class GameModesMenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameModesMenuWindow(QWidget *parent = nullptr);
    ~GameModesMenuWindow();
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnPVsC_clicked();

    void on_btnPVsPOfflane_clicked();

    void on_btnPVsPOnline_clicked();

private:
    Ui::GameModesMenuWindow *ui;
};"""
)))