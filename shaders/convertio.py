from PIL import Image
import numpy as np

path = "C:\\Users\\Gordon\\Desktop\\el_03_01.bmp"

img = Image.open(path)

img = np.array(Image.open(path))

im = Image.fromarray(img)
im.save("your_file.tga")