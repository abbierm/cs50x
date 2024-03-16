def get_name() -> str:
    name = input('What is your name? ')
    return name


def print_response(name: str) -> None:
    print(f"hello, {name}")


if __name__ == "__main__":
    name = get_name()
    print_response(name)
