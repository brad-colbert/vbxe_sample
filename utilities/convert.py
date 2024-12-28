from PIL import Image

# Load the image
image = Image.open('example/winhill.png')

# Print the dimensions of the image
print(f"Image dimensions: {image.size}")


# Define the target size
target_width, target_height = 320, 240

# Calculate the new size preserving the aspect ratio
image_ratio = image.width / image.height
target_ratio = target_width / target_height

if image_ratio > target_ratio:
    # Image is wider than target, fit to width
    new_width = target_width
    new_height = int(target_width / image_ratio)
else:
    # Image is taller than target, fit to height
    new_width = int(target_height * image_ratio)
    new_height = target_height

print(f"New Image dimensions: {new_width, new_height}")

# Resize the image
image = image.resize((new_width, new_height), Image.BILINEAR)

# Create a new image with the target size and a black background
new_image = Image.new('RGB', (target_width, target_height), (0, 0, 0))

# Calculate the position to paste the resized image onto the black background
paste_x = (target_width - image.width) // 2
paste_y = (target_height - image.height) // 2

# Paste the resized image onto the black background
new_image.paste(image, (paste_x, paste_y))

# Replace the original image with the new image
image = new_image


'''
# Calculate the optimal cropping box
image_ratio = image.width / image.height
target_ratio = target_width / target_height

if image_ratio > target_ratio:
    # Image is wider than target, crop the sides
    new_width = int(target_ratio * image.height)
    new_height = image.height
else:
    # Image is taller than target, crop the top and bottom
    new_width = image.width
    new_height = int(image.width / target_ratio)

left = (image.width - new_width) / 2
top = (image.height - new_height) / 2
right = (image.width + new_width) / 2
bottom = (image.height + new_height) / 2

# Crop the image
image = image.crop((left, top, right, bottom))

# Resize the image
image = image.resize((target_width, target_height), Image.BILINEAR)
'''

# Convert the image to use a palette
image = image.convert('P', palette=Image.ADAPTIVE, colors=256)

# Get the palette
palette = image.getpalette()


# Print the palette
# Print the palette with RGB tuples
rgb_palette = [(palette[i], palette[i+1], palette[i+2]) for i in range(0, len(palette), 3)]
print(rgb_palette)

# Get the image data
image_data = image.tobytes()

# Print the first 320 bytes of the image data
# Print the first line of the image data in hexadecimal
first_line_hex = ' '.join(f'{byte:02x}' for byte in image_data[:target_width])
print(first_line_hex)


# Offset the palette entries by one
offset_palette = [0] * 3 + palette[:-3]
#offset_palette = [0, 0, 0] + palette[:-3]

# Offset the image data by one
offset_image_data = bytes((byte + 1) % 256 for byte in image_data)

# Print the offset palette with RGB tuples
offset_rgb_palette = [(offset_palette[i], offset_palette[i+1], offset_palette[i+2]) for i in range(0, len(offset_palette), 3)]
print("\n")
print(offset_rgb_palette)
print("\n")

# Print the first line of the offset image data in hexadecimal
first_line_offset_hex = ' '.join(f'{byte:02x}' for byte in offset_image_data[:target_width])
print(first_line_offset_hex)


# Save the palette to a file with the same basename but with a .PAL extension
palette_filename = 'atr/winhill.pal'
with open(palette_filename, 'wb') as palette_file:
    palette_file.write(bytes(offset_palette))

# Save the raw bytes of the image to a file with the same basename but with a .IMG extension
image_filename = 'atr/winhill.img'
with open(image_filename, 'wb') as image_file:
    image_file.write(offset_image_data)


# Read the palette file
with open(palette_filename, 'rb') as palette_file:
    palette_data = palette_file.read()

# Read the image file
with open(image_filename, 'rb') as image_file:
    image_data = image_file.read()

# Create a new image with the same size and mode
new_image = Image.frombytes('P', (target_width, target_height), image_data)

# Apply the palette to the new image
new_image.putpalette(palette_data)

# Show the new image
new_image.show()

# Show the image
#image.show()