import { Box } from '@mui/system'
import UserSelector from './UserSelector'
import TextBox from './TextBox'
import SendButton from './SendButton'
import LogOff from './LogOff'

interface MessageInputBarProps {
    onTextBoxChange: (textBoxContent: string) => void
    onSendButtonClick: () => void
    onLogOffClick: () => void
    onNewUserSelector: (user: string) => void
    currentSelectedUser: string | null
    message: string
}

function MessageInputBar(
    { onTextBoxChange, onSendButtonClick, onLogOffClick, onNewUserSelector, currentSelectedUser, message }: MessageInputBarProps) {
    return (
        <Box
            sx={{
                width: '100%', // Full width of the parent container
                height: '10%', // 10% height of the parent container
                display: 'flex',
                alignItems: 'stretch', // Stretch children to fill the height
                justifyContent: 'center',
            }}
        >
            <UserSelector
                onNewUserSelector={onNewUserSelector}
                currentSelectedUser={currentSelectedUser}
            />
            <TextBox
                onChange={onTextBoxChange}
                message={message}
            />
            <SendButton
                onClick={onSendButtonClick}
            />
            <LogOff
                onClick={onLogOffClick}
            />
        </Box>
    )
}

export default MessageInputBar