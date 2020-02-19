MAX_PACKET_LENGTH = 100 # TODO: update if necessary

# packet types
PACKET = {
        "DIGITAL_DATA":         239,
        "DIGITAL_READ":         16,
        "ERROR":                255,
        "LED_LOW":              170,
        "LED_HIGH":             85,
        "PIN_INIT":             240 
}

# error codes
ERROR = {
        "INVALID_PACKET":   2,
        "INVALID_PIN":      1
}

