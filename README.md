Running the Project

1. Start the C Backend Server

Open a terminal.

Go to the backend/build folder:

cd backend/build

Run the server:

./server.exe

You should see something like:

STARTING SERVER........
SERVER LISTENING TO PORT 8080

2. Start the Frontend

Open another terminal.

Go to the frontend folder:

cd frontend

Run the frontend:

npm run dev

3. Open the Website

Visit:

http://localhost:3000

4. Upload a File

Click the upload button

Select any file

Upload it

The frontend sends the file to the C backend using HTTP requests.

The backend currently receives:

HTTP headers

multipart/form-data

binary file bytes

The upload/file parsing logic is still under development.
