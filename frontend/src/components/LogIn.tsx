import FormControl, { useFormControl } from '@mui/material/FormControl';
import OutlinedInput from '@mui/material/OutlinedInput';
import FormHelperText from '@mui/material/FormHelperText';
import Box from '@mui/material/Box';
import Button from '@mui/material/Button';
import React from 'react';
import { useState } from 'react';

type LogInProps = {
    onUserLogIn: (userName: string) => void;
};

// Use the type with your component
function LogIn({ onUserLogIn }: LogInProps) {
    const [username, setUserName] = useState("");

    const handleLogin = () => {
        onUserLogIn(username);
    }

    const handleUsernameChange = (event: React.ChangeEvent<HTMLInputElement>) => {
        setUserName(event.target.value);
    }

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
                }}>

                <FormControl sx={{ width: '25ch' }}> { /* Login form */}
                    <HelperText />
                    <OutlinedInput
                        placeholder="Username..."
                        value={username}
                        onChange={handleUsernameChange} /* Keeps track of the input */
                    />
                    <Button type="submit" variant="contained"
                        sx={{ top: 5 }}>Log On</Button>
            </FormControl>
        </Box >
        </form >
    )

}

export default LogIn

// Function that checks if the tex box is foucused. If it is it returns nothing and if it isn't it returns "Enter Username"
function HelperText() {
    const { focused } = useFormControl() || {};

    const helperText = React.useMemo(() => {
        if (!focused) {
            return 'Enter Username';
        }
    }, [focused]);

    return <FormHelperText>{helperText}</FormHelperText>;
}
