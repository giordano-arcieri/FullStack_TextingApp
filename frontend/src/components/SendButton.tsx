import React from 'react';
import Button from '@mui/material/Button';

const SendButton: React.FC<{ onClick: () => void }> = ({ onClick }) => {
    return (
        <Button sx={{ width: '20%', margin: 0.5 }}
        size='large' variant="contained" color="primary" onClick={onClick}>
            Send
        </Button>
    );
};

export default SendButton;
