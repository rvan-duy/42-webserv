import sys


# Python adds a newline to any line printed, 
# so a valid header ends with a \r
def main():
    import json
    print("Content-Type: json")
    print("Content-Length: ",len(sys.argv[1]))
    print("\n")
    input = json.loads(sys.argv[1])
    output = {
        "first_name": input["first_name"].upper(),
        "last_name": input["last_name"].upper()
    }
    print(output)
    exit(0)

if __name__ == "__main__":
    main()