from ultralytics import YOLO

model = YOLO("FruitVeg.pt")
print(model.names)