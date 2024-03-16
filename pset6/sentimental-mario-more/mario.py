while True:
    try:
        height = int(input('Height: '))
        if height > 0 and height <= 8:
            break
    except ValueError:
        continue

for i in range(1, height + 1):
    print_string = (height - i) * ' ' + '#' * i + '  ' + '#' * i
    print(print_string)
