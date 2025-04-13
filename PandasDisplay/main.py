import cv2
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def apply_rotation(img, angle=45):
    h, w = img.shape[:2]
    M = cv2.getRotationMatrix2D((w // 2, h // 2), angle, 1)
    return cv2.warpAffine(img, M, (w, h))

def apply_translation(img, tx=50, ty=30):
    h, w = img.shape[:2]
    M = np.float32([[1, 0, tx], [0, 1, ty]])
    return cv2.warpAffine(img, M, (w, h))

def apply_shear(img, shear_factor=0.3):
    h, w = img.shape[:2]
    M = np.float32([[1, shear_factor, 0], [0, 1, 0]])
    return cv2.warpAffine(img, M, (int(w + shear_factor * h), h))

def apply_scaling(img, scale_x=1.5, scale_y=1.5):
    h, w = img.shape[:2]
    return cv2.resize(img, (int(w * scale_x), int(h * scale_y)))

# Load image
img = cv2.imread('/home/digital101/ImageProcessingBook/FourierTransform/denoise.jpg')  # Replace with your image path
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # Convert for matplotlib

# Apply transformations
rotated = apply_rotation(img)
translated = apply_translation(img)
sheared = apply_shear(img)
scaled = apply_scaling(img)

# Store images in a pandas DataFrame
data = pd.DataFrame({
    "Transformation": ["Original", "Rotation", "Translation", "Shear", "Scaling"],
    "Image": [img, rotated, translated, sheared, scaled]
})

# Display all images using matplotlib
fig, axes = plt.subplots(1, len(data), figsize=(20, 5))
for i, row in data.iterrows():
    axes[i].imshow(row["Image"])
    axes[i].set_title(row["Transformation"])
    axes[i].axis("off")
plt.tight_layout()
plt.show()
