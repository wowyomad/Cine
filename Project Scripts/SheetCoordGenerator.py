import argparse

def generate_coords(texture_width, texture_height, rows, cols, offset=0):
    coords = []
    cell_width = (texture_width + offset) / cols
    cell_height = (texture_height + offset) / rows

    for row in range(rows):
        for col in range(cols):
            x0 = (col * cell_width) / texture_width
            y0 = (row * cell_height) / texture_height
            x1 = ((col + 1) * cell_width) / texture_width
            y1 = ((row + 1) * cell_height) / texture_height

            # Append coordinates as nested lists
            coords.append([
                [x0, y0],
                [x1, y0],
                [x1, y1],
                [x0, y1]
            ])

    return coords

def main():
    parser = argparse.ArgumentParser(description='Generate sprite sheet coordinates in a custom format.')
    parser.add_argument('width', type=float, help='Texture width in pixels')
    parser.add_argument('height', type=float, help='Texture height in pixels')
    parser.add_argument('rows', type=int, help='Number of rows in the sprite sheet')
    parser.add_argument('columns', type=int, help='Number of columns in the sprite sheet')
    parser.add_argument('--offset', type=float, default=0, help='Offset in pixels between sprites (default: 0)')
    parser.add_argument('--output', type=str, default='spritesheet.txt', help='Output file name (default: spritesheet.txt)')

    args = parser.parse_args()

    coords = generate_coords(args.width, args.height, args.rows, args.columns, args.offset)

    # Open output file
    with open(args.output, 'w') as outfile:
        for coord in coords:
            outfile.write('- Coords:\n')
            for c in coord:
                outfile.write(f'    - [{c[0]:.10f}, {c[1]:.10f}]\n')

    print(f'Successfully generated sprite sheet coordinates in {args.output}.')

if __name__ == '__main__':
    main()
