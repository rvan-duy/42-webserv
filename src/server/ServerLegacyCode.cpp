#include <Server.hpp>

/**************************************************/
/* LEGACY CODE                                    */
/*                                                */
/* This code is not used anymore, but is kept     */
/* here for reference                             */
/**************************************************/

/*
 * Accept an incoming connection
 */
void Server::accept_connection()
{
    Logger &logger = Logger::getInstance();
    const socklen_t sock_len = sizeof(_servaddr); // size of socket address structure

    { // This block is just for the logger
        std::ostringstream ss;
        ss << "Waiting for connections on port " << _port;
        logger.log(ss.str());
    }

    /**************************************************/
    /* Accept an incoming connection                  */
    /* - Blocks until a connection is present         */
    /* - Because the accept() function expects a      */
    /* sockaddr structure, it needs to be cast to the */
    /* correct type, the sockaddr_in structure is a   */
    /* "subclass" of sockaddr, so we can cast it to a */
    /* sockaddr structure                             */
    /**************************************************/

    if ((_accepted = accept(_fd, (struct sockaddr *)&_servaddr, (socklen_t *)&sock_len)) == -1)
    {
        logger.error("Socket accept failed: " + std::string(strerror(errno)));
        throw std::runtime_error("Socket accept failed: " + std::string(strerror(errno)));
    }

    logger.log("New connection accepted");

    /**************************************************/
    /* Read data from the incoming connection         */
    /**************************************************/

    logger.log("Reading data from connection");

    bzero(_buffer, sizeof(_buffer));
    ssize_t bytes_read = read(_accepted, _buffer, sizeof(_buffer));
    if (bytes_read == -1)
    {
        logger.error("Socket read failed: " + std::string(strerror(errno)));
        throw std::runtime_error("Socket read failed: " + std::string(strerror(errno)));
    }

    if (bytes_read == 0)
    {
        logger.log("Socket read 0 bytes, ignoring");
        return;
    }

    { // This block is just for the logger
        std::ostringstream ss;
        ss << "Successfully read " << bytes_read << " bytes from socket";
        logger.log(ss.str());
    }

    /**************************************************/
    /* Log the incoming data                          */
    /**************************************************/

    logger.log("Received data:\n---------------------------\n" + std::string(_buffer) +
               "\n---------------------------\n");

    /**************************************************/
    /* End of data receiver, data is now stored       */
    /* inside the object:                             */
    /* - _buffer: contains the data                   */
    /* - _accepted: contains the accepted socket      */
    /**************************************************/

    return;
}

/*
 * Wait for incoming connections
 */
void Server::wait_for_connections()
{
    Logger &logger = Logger::getInstance();
    const socklen_t sock_len = sizeof(_servaddr); // size of socket address structure
    int new_fd;                                   // file descriptor for new socket
    char buffer[100000] = {0};                    // buffer to read incoming data into

    /**************************************************/
    /* Start accepting incoming connections           */
    /**************************************************/

    while (1)
    {
        { // This block is just for the logger
            std::ostringstream ss;
            ss << "Waiting for connections on port " << _port;
            logger.log(ss.str());
        }

        /**************************************************/
        /* Accept an incoming connection                  */
        /* - Blocks until a connection is present         */
        /* - Because the accept() function expects a      */
        /* sockaddr structure, it needs to be cast to the */
        /* correct type, the sockaddr_in structure is a   */
        /* "subclass" of sockaddr, so we can cast it to a */
        /* sockaddr structure                             */
        /**************************************************/

        if ((new_fd = accept(_fd, (struct sockaddr *)&_servaddr, (socklen_t *)&sock_len)) == -1)
        {
            logger.error("Socket accept failed: " + std::string(strerror(errno)));
            throw std::runtime_error("Socket accept failed: " + std::string(strerror(errno)));
        }

        /**************************************************/
        /* Read data from the incoming connection         */
        /**************************************************/

        logger.log("Reading data from socket");

        bzero(buffer, sizeof(buffer));
        ssize_t bytes_read = read(new_fd, buffer, sizeof(buffer));
        if (bytes_read == -1)
        {
            logger.error("Socket read failed: " + std::string(strerror(errno)));
            throw std::runtime_error("Socket read failed: " + std::string(strerror(errno)));
        }

        if (bytes_read == 0)
        {
            logger.log("Socket read 0 bytes, ignoring");
            continue;
        }

        { // This block is just for the logger
            std::ostringstream ss;
            ss << "Successfully read " << bytes_read << " bytes from socket";
            logger.log(ss.str());
        }

        /**************************************************/
        /* Log the received message                       */
        /**************************************************/

        logger.log("Received request\n---------------------------\n" + std::string(buffer) +
                   "\n---------------------------\n");

        /**************************************************/
        /* Send response to the client                    */
        /**************************************************/

        {
            std::string msg(buffer);
            std::string tmp_index("index.html");

            buildRequest(msg, new_fd);
            HttpResponse response(_requests[new_fd]->constructResponse(*this, tmp_index)); // TODO: make root(done!) configurable, not hardcoded, same for index

            std::string response_str = response.toStr();

            if (write(new_fd, response_str.c_str(), response_str.length()) == -1)
            {
                logger.error("Socket write failed: " + std::string(strerror(errno)));
                throw std::runtime_error("Socket write failed: " + std::string(strerror(errno)));
            }
        }

        /**************************************************/
        /* Close the connection                           */
        /**************************************************/

        if (close(new_fd) == -1)
        {
            logger.error("Socket close failed: " + std::string(strerror(errno)));
            throw std::runtime_error("Socket close failed: " + std::string(strerror(errno)));
        }
    }
}

/**************************************************/
/* END OF LEGACY CODE                             */
/**************************************************/
