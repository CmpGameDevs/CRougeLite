from PIL import Image
import os

def split_sprite_sheet(image_path, tile_width, tile_height, output_dir):
    # Open the sprite sheet image
    sprite_sheet = Image.open(image_path)
    sheet_width, sheet_height = sprite_sheet.size

    # Calculate the number of tiles horizontally and vertically
    tiles_x = sheet_width // tile_width
    tiles_y = sheet_height // tile_height

    # Create the output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Split the sprite sheet into individual tiles
    for y in range(tiles_y):
        for x in range(tiles_x):
            # Calculate the position of the current tile
            left = x * tile_width
            top = y * tile_height
            right = left + tile_width
            bottom = top + tile_height

            # Crop the tile from the sprite sheet
            tile = sprite_sheet.crop((left, top, right, bottom))

            # Save the tile to the output directory
            tile_path = os.path.join(output_dir, f"slime_{y}_{x}.png")
            tile.save(tile_path)

    print(f"All tiles have been saved to {output_dir}")

if __name__ == "__main__":
    # Example usage
    sprite_sheet_path = "./slime_purple.png"
    tile_width = 16  # Replace with the actual tile width
    tile_height = 16  # Replace with the actual tile height
    output_directory = "slime"

    split_sprite_sheet(sprite_sheet_path, tile_width, tile_height, output_directory)
