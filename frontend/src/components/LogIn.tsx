import FormControl, { useFormControl } from '@mui/material/FormControl';
import OutlinedInput from '@mui/material/OutlinedInput';
import FormHelperText from '@mui/material/FormHelperText';
import Box from '@mui/material/Box';
import Button from '@mui/material/Button';
import React from 'react';

function LogIn() {
    const { focused } = useFormControl() || {};

    const helperText = React.useMemo(() => {
        if (!focused) {
            return 'Enter Username';
        }
    }, [focused]);

    return <FormHelperText>{helperText}</FormHelperText>;
}

export default function UseFormControl() {
    const [username, setUserName] = React.useState('');

    const handleLogin = (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault();
        console.log("New Login:", username); // API call that makes a new user and brings them to the main space
    };

    const handleUsernameChange = (event: React.ChangeEvent<HTMLInputElement>) => {
        setUserName(event.target.value);
    };

    return (
        <form noValidate autoComplete="off" onSubmit={handleLogin}>
            { /* The hole website will be this box, and it will be centered.
            The box contains a login form that upon submission an API will be called
            that brings the new user to the main space */ }
            <Box
                sx={{
                    width: '100vw', // Verticle position 
                    height: '70vh', // Horizontal position
                    position: 'absolute', // Ensure it's not affected by any other elements
                    top: 0, // Padding at the top
                    left: 0, // Padding at the left
                    bottom: 0, // Padding at the bottom
                    right: 0, // Padding at the right
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                }}
            >
                <FormControl sx={{ width: '25ch' }}> { /* Login form */}
                    <Box
                        sx={{
                            width: '100%', display: 'flex', flexDirection: 'column', gap: 1
                        }}>
                        <LogIn />
                        <OutlinedInput
                            placeholder="Username..."
                            value={username}
                            onChange={handleUsernameChange} /* Keeps track of the input */
                        />
                        <Button type="submit" variant="contained">Log On</Button>
                    </Box>
                </FormControl>
            </Box >
        </form >
    );
}


