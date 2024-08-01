import { useState, useEffect } from 'react';
import Box from '@mui/material/Box';
import Typography from '@mui/material/Typography';
import Paper from '@mui/material/Paper';
import { styled } from '@mui/material/styles';
import Button from '@mui/material/Button';

interface MessagesProps {
    username: string;
}

interface Message {
    content: string;
    sender: string;
}

const Messages = ({ username }: MessagesProps) => {
    const [messages, setMessages] = useState<Message[]>([]);

    const getMessages = async (): Promise<Message[]> => {
        const URL: string = 'http://localhost:9080/getMessages/' + username;
        console.log('Fetching messages for:', JSON.stringify({ username: username }));
        try {
            const response = await fetch(URL, {
                method: 'GET',
            });
            if (response.status === 200) {
                const data = await response.json();
                console.log('Fetched messages:', data);
                return data.messages as Message[];
            } else {
                console.error('Failed to fetch messages');
            }
        } catch (error) {
            console.error('Error fetching messages:', error);
        }
        return [];
    };

    const fetchMessages = async () => {
        const messages = await getMessages();
        setMessages(messages);
    };

    useEffect(() => {
        fetchMessages();
    }, [username]); // Call API when component mounts or `username` changes

    // Polling for periodic updates
    useEffect(() => {
        const intervalId = setInterval(fetchMessages, 3000); // Poll every 30 seconds
        return () => clearInterval(intervalId); // Cleanup interval on component unmount
    }, [username]);

    return (
        <Box
            sx={{
                width: '100%', height: '90%', overflow: 'auto', border: '1px solid black',
            }}
        >
            <h1>Username: {username}</h1>
            <Button variant="contained" color="primary" onClick={fetchMessages} sx={{ marginBottom: 2, marginLeft: 4 }}>
                Refresh Messages
            </Button>
            {messages.map((message, index) => (
                <MessageBubble key={index} isOwnMessage={message.sender === username}>
                    <Typography variant="body1" component="span">{message.content}</Typography>
                    <Typography variant="caption" component="span" sx={{ display: 'block', marginTop: 0.5 }}>
                        {message.sender === username ? 'me' : 'From: ' + message.sender}
                    </Typography>
                </MessageBubble>
            ))}
        </Box>
    );
};

const MessageBubble = styled(Paper, {
    shouldForwardProp: (prop) => prop !== 'isOwnMessage'
})<{ isOwnMessage: boolean }>(({ isOwnMessage }) => ({
    marginBottom: '10px',
    marginLeft: isOwnMessage ? 'auto' : '20px',
    marginRight: isOwnMessage ? '20px' : 'auto',
    padding: '10px',
    minWidth: '100px', 
    maxWidth: '60%', 
    width: 'fit-content', 
    alignSelf: isOwnMessage ? 'flex-end' : 'flex-start',
    backgroundColor: isOwnMessage ? '#dcf8c6' : '#9ae4ff',
    borderRadius: '10px',
    borderTopRightRadius: isOwnMessage ? '0px' : '10px',
    borderTopLeftRadius: isOwnMessage ? '10px' : '0px',
    overflowWrap: 'break-word',
    position: 'relative',
    '::after': {
        content: '""',
        position: 'absolute',
        width: 0,
        height: 0,
        borderStyle: 'solid',
        borderWidth: isOwnMessage ?  '0 0 15px 10px' : '0 10px 15px 0',
        backgroundColor: '#ffffff',
        top: 0,
        left: isOwnMessage ? 'auto' : '-10px',
        right: isOwnMessage ? '-10px' : 'auto',
        borderColor: isOwnMessage ? 'transparent transparent transparent #dcf8c6' : 'transparent #9ae4ff transparent transparent',
    }
}));

export default Messages;
