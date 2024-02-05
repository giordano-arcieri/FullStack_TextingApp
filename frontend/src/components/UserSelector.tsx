import React from 'react';
import Button from '@mui/material/Button';

const UserSelector = ({ onClick, currentSelectedUser }: { onClick: () => void, currentSelectedUser: string | null }) => {
    return (
        <Button sx={{  width: '20%', margin: 0.5}}
            variant="contained" color="primary" onClick={onClick}>
            {currentSelectedUser ? currentSelectedUser : 'Select User'}
        </Button>
    );
};

export default UserSelector;
