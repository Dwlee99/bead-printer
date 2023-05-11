from PIL import Image
import numpy as np

COLOR_MAP = {
    (0, 0, 255): "blue",
    (255, 255, 0): "yellow",
    (0, 0, 0): "black",
    (255, 255, 255): "white",
    (128, 128, 128): "grey",
    (128, 0, 128): "purple",
    (255, 0, 0): "red",
    (255, 192, 203): "pink",
    (165, 42, 42): "brown"
}


# Map the colors to the nearest color in the color map
def get_nearest_color(color):
    min_distance = float("inf")
    nearest_color = None
    for cmap_color, cmap_name in COLOR_MAP.items():
        distance = ((cmap_color[0]-color[0])**2 +
                    (cmap_color[1]-color[1])**2 +
                    (cmap_color[2]-color[2])**2)**0.5
        if distance < min_distance:
            min_distance = distance
            nearest_color = cmap_name
    if min_distance > 100:
        # No matching color found in COLOR_MAP
        nearest_color = None
    return nearest_color

input_image = Image.open("ProfilePic.jpg")
resized_image = input_image.resize((8, 8), resample=Image.BOX)
palette_image = resized_image.convert("P", palette=Image.ADAPTIVE, colors=3)
palette = palette_image.getpalette()
color1 = tuple(palette[:3]) #this is rgb so 3 values
color2 = tuple(palette[3:6])

color1_name = get_nearest_color(color1)
color2_name = get_nearest_color(color2)
color3 = None

print(f"The two colors in the palette are: {color1_name} and {color2_name}")


# Create an array to store the pixel art
pixel_art = np.empty((8, 8), dtype='object')

# Iterate through each pixel and map it to the nearest color
for x in range(8):
    for y in range(8):
        color = palette_image.getpixel((x, y))
        if color == 0:
            pixel_art[x, y] = "COLOR0"
        elif color == 1:
            pixel_art[x, y] = "COLOR1"
        else:
            pixel_art[x, y] = "NONE"

array_str = "{"

for i in range(8):
    array_str += "{"
    for j in range(8):
        array_str += pixel_art[i][j] + ","
    array_str += "}" + ","
array_str += "}"

print(array_str)