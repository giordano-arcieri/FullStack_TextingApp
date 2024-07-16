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

    useEffect(() => {
        const fetchOnlineUsers = async () => {
            try {
                const users = await getOnlineUsers();
                setOnlineUsers(users);
            } catch (err) {
                throw err;
            }
        };

        fetchOnlineUsers();
    }, []);

    const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
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
        const serverUrl = 'http://localhost:8080/get_online_users';
    
     
        console.log('Attempting to fetch online users from:', serverUrl);
        const response = await fetch(serverUrl, {
            method: 'GET',
    
        });

        let textResponse: string = await response.text(); // Get the raw text response
        console.log('Raw Response:', textResponse);
        

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        // const data = JSON.parse(textResponse); // Parse the JSON
        // console.log('Fetched Online Users:', data.users);
        // return data.users;
        return ["textResponse"]

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

