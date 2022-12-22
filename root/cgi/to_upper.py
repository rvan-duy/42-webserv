import sys

# Python adds a newline to any line printed, 
# so a valid header ends with a \r
def main():
    import json
    input = json.loads(sys.argv[1])
    output = input["first_name"].upper() + " " + input["last_name"].upper()

    print("Content-Type: text/plain")
    print("Content-Length: 15")
    # print("Content-Length: 3",len(sys.argv[1]))
    print("\n")
    print(output)
    exit(0)

if __name__ == "__main__":
    main()