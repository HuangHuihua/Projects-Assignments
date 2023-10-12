import React from 'react';
import Button from '@mui/material/Button';

function CopyButton ({ sessionID }) {
  const handleCopyClick = async () => {
    try {
      await navigator.clipboard.writeText('http://localhost:3000/PlayerPlay/' + sessionID);
      alert('Copy success!');
    } catch (err) {
      console.error('Fail to copy: ', err);
    }
  };

  return (
    <Button onClick={handleCopyClick}>
      Copy
    </Button>
  );
}

export default CopyButton;
