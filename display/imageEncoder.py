from PIL import Image

# Set threshold value (0 to 255)
threshold = 10

def image_to_2D_array(image_path):
    # Open image
    img = Image.open(image_path)

    # Convert image to RGB format
    img = img.convert("RGB")

    # Get image dimensions
    width, height = img.size

    # Initialize 2D array
    pixel_array = []

    for y in range(height):
        row = []
        for x in range(width):
            # Get RGB value of pixel
            r, g, b = img.getpixel((x, y))

            # Check if the color is close to black
            if r < threshold and g < threshold and b < threshold:
                r, g, b = 0, 0, 0  # Make the pixel black


            # Convert RGB to GRB and then to hex
            hex_value = (r << 16) + (g << 8) + b
            hex_value = (g << 16) + (r << 8) + b

            # Convert integer to hex string in 0xFF0000 format
            hex_str = '0x{:06X}'.format(hex_value)

            # Append hex string to row
            row.append(hex_str)

        # Append row to 2D array
        pixel_array.append(row)

    return pixel_array

# Example usage
image_path = "fire.png"
pixel_array = image_to_2D_array(image_path)

def write_to_file(pixel_array, file_path):
    with open(file_path, 'w') as f:
        for row in pixel_array:
            row_str = ','.join(row)
            f.write(f'{{{row_str}}},\n')

# Write to file
write_to_file(pixel_array, 'test.txt')

print('done')
