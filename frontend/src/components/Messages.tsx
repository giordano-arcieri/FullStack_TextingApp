import React from 'react';
import Box from '@mui/material/Box';

interface MessagesProps {
    username: string;
}

const Messages = ({username}: MessagesProps) => {
    return (
        <Box>
            <h1>{username}</h1>
            {/* Your content goes here */}
        </Box>
    );
};

export default Messages;
