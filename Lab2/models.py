from sqlalchemy import Column, Integer, String, DateTime
from database import Base
from datetime import datetime

class Image(Base):
    __tablename__ = "images"
    id = Column(Integer, primary_key=True, index=True)
    filename = Column(String, unique=True, index=True)
    device_id = Column(String, index=True)
    timestamp = Column(DateTime, default=datetime.utcnow)

class User(Base):
    __tablename__ = "users"
    id = Column(Integer, primary_key=True, index=True)
    username = Column(String, unique=True)
    password_hash = Column(String)
    role = Column(String)
