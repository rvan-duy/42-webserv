import time

# Python adds a newline to any line printed, 
# so a valid header ends with a \r
def main():
    print("test: hi\r")
    print("test : no\r")
    print("\r")
    print("body")
    exit(0)

if __name__ == "__main__":
    main()