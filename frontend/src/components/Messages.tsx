import React from 'react';
import Box from '@mui/material/Box';

interface MessagesProps {
    username: string;
}

const Messages = ({ username }: MessagesProps) => {
    return (
        <Box
            sx={{
                width: '100%', height: '90%', overflow: 'auto', border: '1px solid black',
            }}
        >
            <h1>{username}</h1>
        </Box>
    );
};

export default Messages;
