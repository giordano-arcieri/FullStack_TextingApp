import React from 'react';
import Button from '@mui/material/Button';

const LogOff: React.FC<{ onClick: () => void }> = ({ onClick }) => {

    return (
        <Button sx={{ width: '10%', margin: 0.5 }} 
        variant="contained" onClick={onClick}>
            Log Off
        </Button>
    );
};

export default LogOff;
