from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

SQLALCHEMY_DATABASE_URL = "sqlite:///./test.db"

engine = create_engine(SQLALCHEMY_DATABASE_URL, connect_args={"check_same_thread": False})
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base = declarative_base()

def save_image_metadata(device_id, filename, timestamp):
    from models import Image
    db = SessionLocal()
    print("🔔 Saving image metadata to database")
    image = Image(device_id=device_id, filename=filename, timestamp=timestamp)
    db.add(image)
    db.commit()
    db.close()
