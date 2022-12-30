
# Python adds a newline to any line printed, 
# so a valid header ends with a \r
def main():
    print("test: hi")
    print("test : no")
    print("\n")
    json = {
        "message": "hi"
    }
    print(json)
    exit(0)

if __name__ == "__main__":
    main()