Server will wait for new clients
upon new client:
    Make new thread to take care of client.
    if client asks for users
        send users
    if client ask for messages from user
        give messages from user (this is a file in the following format)
            Sender Time Message
            Sender Time Message
                Sender = A bool, either 0 for message was sent to the user, or 1 for message was recived by the user
                Time = This will be in military format. DD/MM/YEAR 00:00
                Message = This will be the contenst of the message
                Example (User1_User2.txt):
                    0 1/1/2024 00:10 Hello, How are you?
                    1 1/1/2024 00:12 I am good! How are you?
    if client ask to send a message
        form message, with time and date
        look if file already exist by checking every file if it is = to sender_reciver or reciver_sender
        if not create file
        change 0 or 1 at the beginning of the message
        append the message to the file name
