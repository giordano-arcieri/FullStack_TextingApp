import React, { useEffect, useState } from 'react';
import Button from '@mui/material/Button';
import Menu from '@mui/material/Menu';
import MenuItem from '@mui/material/MenuItem';

interface UserSelectorProps {
    currentSelectedUser: string | null;
    onNewUserSelector: (user: string) => void;
}

const UserSelector: React.FC<UserSelectorProps> = ({ currentSelectedUser, onNewUserSelector }) => {

    const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null);
    const open = Boolean(anchorEl);
    const [onlineUsers, setOnlineUsers] = useState<string[]>([]);

    const fetchOnlineUsers = async () => {
        try {
            const users = await getOnlineUsers();
            setOnlineUsers(users);
        } catch (err) {
            throw err;
        }
    };

    const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
        fetchOnlineUsers();
        setAnchorEl(event.currentTarget);
    };

    const handleClose = () => {
        setAnchorEl(null);
    };

    const handleMenuItemClick = (user: string) => {
        setAnchorEl(null);
        onNewUserSelector(user); // Call onSelectUser prop with selected user
    };

    async function getOnlineUsers(): Promise<string[]> {

        // API call to get current online users
        try {
            const response = await fetch('http://localhost:9080/get_online_users', {
                method: 'GET',
            });
            if (response.status === 200) {
                const data = await response.json();
                console.log('Fetched online users:', data["online_users"]);
                return data["online_users"];
            } else {
                console.error('Failed to fetch online users');
            }
        } catch (error) {
            console.error('Error fething online users:', error);
        }
        return [];
    }

    return (
        <>
            <Button
                id="basic-button"
                aria-controls={open ? 'basic-menu' : undefined}
                aria-haspopup="true"
                aria-expanded={open ? 'true' : undefined}
                onClick={handleClick}
                sx={{ width: '20%', margin: 0.5 }}
                variant="contained"
                color="primary"
            >
                {currentSelectedUser || 'Select User'}
            </Button>
            <Menu
                id="basic-menu"
                anchorEl={anchorEl}
                open={open}
                onClose={handleClose}
                MenuListProps={{
                    'aria-labelledby': 'basic-button',
                }}
            >
                {onlineUsers.map((item) => (
                    <MenuItem key={item} onClick={() => handleMenuItemClick(item)}>
                        {item}
                    </MenuItem>
                ))}
            </Menu>
        </>
    );
};

export default UserSelector;

