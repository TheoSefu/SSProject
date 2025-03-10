import cv2
import os

def resize_image(path, width=512):
    image = cv2.imread(path)
    h, w = image.shape[:2]
    ratio = width / w
    resized = cv2.resize(image, (width, int(h * ratio)))
    cv2.imwrite(path, resized)

def apply_grayscale(path):
    image = cv2.imread(path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    cv2.imwrite(path.replace(".jpg", "_gray.jpg"), gray)

def edge_detection(path):
    image = cv2.imread(path)
    edges = cv2.Canny(image, 100, 200)
    cv2.imwrite(path.replace(".jpg", "_edges.jpg"), edges)
