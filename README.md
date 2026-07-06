# ⚡ Huffman File Compressor

A full-stack, lossless file compression web application implementing the **Huffman Coding algorithm** in **C++**, with a lightweight **Flask** web interface for seamless compression and decompression.

[🌐 Live Demo](https://huffman-compressor-5rzd.onrender.com) 

---

## 🚀 Quick Look

| Core Features | Technical Overview |
| :--- | :--- |
| • Lossless compression & decompression<br>• Real-time efficiency metrics<br>• Automated temporary file cleanup<br>• Supported Formats: `.txt`, `.csv`, `.pdf`, `.png`, `.jpg`, `.docx`, `.bin` | **Backend:** C++17, Flask, Gunicorn<br>**Frontend:** HTML5, CSS3, JavaScript, Bootstrap 5<br>**Deployment:** Render Cloud |

### 📊 Performance Benchmarks

* **Sample Text File (`.txt`):** Reduced to **~59%** of its original size.
* **Sample CSV File (`.csv`):** Reduced to **~62%** of its original size.

> **Note:** Results vary depending on file content and data redundancy. Already-compressed formats (PNG, JPG, PDF, ZIP, etc.) may show little or no size reduction.

---

## 🧠 How it Works


1. **Frequency Analysis**  
   The input file is scanned byte-by-byte to calculate the frequency of every unique character.

2. **Huffman Tree Construction**  
   A priority queue is used to build the Huffman Tree, where characters with lower frequencies are placed deeper in the tree and frequent characters receive shorter paths.

3. **Code Generation**  
   Each character is assigned a unique variable-length binary code by traversing the Huffman Tree, ensuring lossless encoding.

4. **Compression**  
   The original file is converted into a compressed bitstream using the generated Huffman codes. The frequency table is stored in the compressed file so it can be reconstructed later.

5. **Decompression**  
   During decompression, the stored frequency table is used to rebuild the Huffman Tree. The encoded bitstream is then decoded to recreate the original file exactly.
   
---

## ⚠️ Limitations

Although Huffman Coding is an efficient lossless compression algorithm, it has some practical limitations:

- **Compression depends on the input data.** Files with repetitive patterns (such as `.txt` and `.csv`) usually achieve better compression, while files with more random data may show lower compression ratios.

- **Already compressed files gain little or no benefit.** Formats such as `.png`, `.jpg`, `.pdf`, `.zip`, and `.mp4` already use compression techniques internally, so applying Huffman Coding again often provides minimal size reduction and may even slightly increase the file size.

- **Metadata introduces overhead.** To ensure lossless decompression, the compressed file stores the frequency table required to reconstruct the Huffman Tree. For very small files, this additional metadata can make the compressed file slightly larger than the original.

---

## 👨‍💻 Author

**Jagdish**

Electrical Engineering Undergraduate  
IIT Gandhinagar

⭐ If you found this project useful, consider giving it a star on GitHub!