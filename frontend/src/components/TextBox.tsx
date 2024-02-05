import { TextField } from "@mui/material"
import Box from '@mui/material/Box';

export default function TextBox({ onChange }: { onChange: (value: string) => void }) {
    return (
        <TextField
            sx={{
                width: '50%',
                height: '100%',
                margin: 0.5,
                '& .MuiOutlinedInput-root': { height: '90%' }, // Adjust inner input height
            }}
            id="outlined-basic"
            label="Text!"
            variant="outlined"
            onChange={(e) => onChange(e.target.value)}
        />
    )
}



