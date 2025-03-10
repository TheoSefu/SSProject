from fastapi import FastAPI, Depends
from mqtt_client import start_mqtt_client
from database import Base, engine, SessionLocal
from models import Image
from schemas import ImageOut

Base.metadata.create_all(bind=engine)

app = FastAPI()

@app.on_event("startup")
def start_services():
    start_mqtt_client()

@app.get("/images", response_model=list[ImageOut])
def list_images():
    Base.metadata.create_all(bind=engine)
    db = SessionLocal()
    images = db.query(Image).all()
    db.close()
    return images
