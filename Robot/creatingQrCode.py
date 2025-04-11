import qrcode

# Create a QR code object with a larger size and higher error correction
qr = qrcode.QRCode(version=1, box_size=10, border=4, error_correction=qrcode.constants.ERROR_CORRECT_H)

# Define the data to be encoded in the QR code
data = "745"


# Add the data to the QR code object
qr.add_data(data)

# Make the QR code
qr.make(fit=True)

# Create an image from the QR code with a black fill color and white background
img = qr.make_image(fill_color="black", back_color="white")

# Save the QR code image
img.save(f"qr_code_{data}.png")


