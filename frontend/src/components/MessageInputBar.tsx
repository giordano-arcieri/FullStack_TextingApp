import React from 'react'
import { Box } from '@mui/system'
import UserSelector from './UserSelector'
import TextBox from './TextBox'
import SendButton from './SendButton'
import LogOff from './LogOff'

interface MessageInputBarProps {
    onTextBoxChange: () => void
    onSendButtonClick: () => void
    onLogOffClick: () => void
    onUserSelectorClick: () => void
    currentSelectedUser: string | null
}

function MessageInputBar(
    { onTextBoxChange, onSendButtonClick, onLogOffClick, onUserSelectorClick, currentSelectedUser }: MessageInputBarProps) {
    return (
        <Box
            sx={{
                width: '100%', // Full width of the parent container
                height: '100%', // Full height of the parent container
                display: 'flex',
                alignItems: 'stretch', // Stretch children to fill the height
                justifyContent: 'center',
            }}
        >
            <UserSelector
                onClick={onUserSelectorClick}
                currentSelectedUser={currentSelectedUser}
            />
            <TextBox
                onChange={onTextBoxChange}
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