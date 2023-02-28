# <p align='center'>FileManager</p>

Консольный файловый менеджер для Ubuntu (без ncurses). Поддерживает удаление/копирование/вырезание/вставку файла, изменение размера окна терминала (отображение адаптируется при следующем
обновлении экрана), переключение показа скрытых файлов. Подсветка: синим цветом - директории, бирюзовым - символьные ссылки, жёлтым - FIFO.

# Клавиши

- стрелки вверх-вниз для перемещения по файлам

- enter - зайти в директорию

- d - удалить выбранный файл

- c - скопировать выбранный файл

- x - вырезать выбранный файл

- v - вставить файл в текущую директорию

- h - отобразить/спрятать скрытые файлы

- q - завершить программу


# Setup

```
mkdir build
cd build
make
./main
```

# TODO

- обработка ошибок

- правильное форматирование кириллицы

- открытие файлов по расширению?

- исправить миллион потенциальных багов
