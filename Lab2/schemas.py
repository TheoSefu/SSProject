from pydantic import BaseModel
from datetime import datetime

class ImageOut(BaseModel):
    device_id: str
    filename: str
    timestamp: datetime

    class Config:
        orm_mode = True
