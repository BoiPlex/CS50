def main():
    while True:
        try:
            height = int(input("Height: "))
        except ValueError:
            print("Height must be an integer")
            continue

        if (height <= 8 and height >= 1 and type):
            break

        print("Height must be between 1 to 8 inclusive")

    for i in range(height):
        spaces = " " * (height - i - 1)
        bricks = "#" * (i + 1)
        print(spaces + bricks + "  " + bricks)


if __name__ == "__main__":
    main()