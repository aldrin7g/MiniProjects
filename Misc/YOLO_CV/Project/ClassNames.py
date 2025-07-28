from ultralytics import YOLO

model = YOLO("helnumbest.pt")
print(model.names.values())