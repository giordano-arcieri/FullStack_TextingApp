import { TextField } from "@mui/material"

export default function TextBox({ onChange, message }: { onChange: (value: string) => void, message: string }) {
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
            content={message}
            onChange={(e) => onChange(e.target.value)}
        />
    )
}



