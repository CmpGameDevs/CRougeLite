from PIL import Image
import os
import sys


def split_sprite_sheet(image_path, tile_width, tile_height, output_dir, offset):
    try:
        sprite_sheet = Image.open(image_path)

    except Exception:
        print("An error occured while opening the image, please make sure that\
        it exists and it is in the png form")
        exit(1)

    # Open the sprite sheet image
    sheet_width, sheet_height = sprite_sheet.size

    # Calculate the number of tiles horizontally and vertically
    tiles_x = sheet_width // tile_width
    tiles_y = sheet_height // tile_height

    # Create the output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    should_break = False
    # Split the sprite sheet into individual tiles
    for y in range(tiles_y):
        for x in range(tiles_x):

            # Calculate the position of the current tile
            left = x * tile_width
            top = y * tile_height + y * offset
            right = left + tile_width
            bottom = top + tile_height

            if top >= sheet_height:
                should_break = True
                break

            # Crop the tile from the sprite sheet
            tile = sprite_sheet.crop((left, top, right, bottom))

            # Save the tile to the output directory
            tile_path = os.path.join(
                output_dir, f"{output_directory}_{y}_{x}.png")
            tile.save(tile_path)

        if should_break:
            break

    print(f"All tiles have been saved to {output_dir}")


if __name__ == "__main__":
    # Example usage
    if len(sys.argv) != 5:
        print("Usage: ./script_name sprite_sheet_name.png tile_width tile_height offset")
        exit(1)

    sprite_sheet_name = sys.argv[1]
    sprite_sheet_path = f"./{sprite_sheet_name}"

    tile_width = int(sys.argv[2])  # Replace with the actual tile width
    tile_height = int(sys.argv[3])  # Replace with the actual tile height

    offset = int(sys.argv[4])

    output_directory = sprite_sheet_name.split(".")[0]

    split_sprite_sheet(sprite_sheet_path, tile_width,
                       tile_height, output_directory, offset)
