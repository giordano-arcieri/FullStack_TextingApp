import React from 'react';
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

    const getOnlineUsers = () => {
        // API call to get online users
        return ['User 1', 'User 2', 'User 3', 'User 4'];
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
                {getOnlineUsers().map((item) => (
                    <MenuItem key={item} onClick={() => handleMenuItemClick(item)}>
                        {item}
                    </MenuItem>
                ))}
            </Menu>
        </>
    );
};

export default UserSelector;

 