"use client"

import { useState } from "react"

export default function Home(){
  const [file , setFile ] = useState<File | null>(null)
  const handleUpload = async() => {
    if (!file){
      alert("please upload a file")
      return
    }
    const formData = new FormData()
    formData.append("file" , file)

    try {
      const response = await fetch("http://127.0.0.1:8080/" , {
        method : "POST",
        body : formData
      })

      setFile(null)

      const data = await response.text()
      console.log(data)
      
    } catch (error) {
      console.error(error)
    }

  }

  return(
    <div>
      <h1>UPLOAD FILES</h1>
      <input type="file" onChange={(e) => {
        if (e.target.files && e.target.files.length >= 0){
          setFile(e.target.files[0])
        }
      }}/>
      <button onClick={handleUpload}>Upload</button>

    </div>
  )
}