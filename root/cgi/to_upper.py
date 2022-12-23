import sys

# Python adds a newline to any line printed, 
# so a valid header ends with a \r
def main():
    import json
    input = json.loads(sys.argv[1])
    try:
        firstname = input["first_name"].upper()
    except KeyError as error:
        firstname = "none"
    try:
        lastname = input["last_name"].upper()
    except KeyError as error:
        lastname = "none"
    output = firstname + " " + lastname

    # Headers
    print("Content-Type: text/plain")
    print("Content-Length:" + str(len(output)))

    # Split body from headers
    print("")
    
    # Body

    print(output)
    exit(0)

if __name__ == "__main__":
    main()