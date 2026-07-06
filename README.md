# ⚡ Huffman File Compressor

A full-stack, lossless file compression web application implementing the **Huffman Coding algorithm** in **C++**, with a lightweight **Flask** web interface for seamless compression and decompression.

[🌐 Live Demo](https://huffman-compressor-5rzd.onrender.com) | [👨‍💻 Author Profile](https://github.com/Jagdish410)

---

## 🚀 Quick Look

| Core Features | Technical Overview |
| :--- | :--- |
| • Lossless compression & decompression<br>• Real-time efficiency metrics<br>• Automated temporary file cleanup<br>• Supported Formats: `.txt`, `.csv`, `.pdf`, `.png` | **Backend:** C++17, Flask, Gunicorn<br>**Frontend:** JavaScript,, Bootstrap 5<br>**Deployment:** Render Cloud |

### 📊 Performance Benchmarks
* **Plain Text (`.txt`):** Compresses down to **~59%** of original size.
* **Structured Data (`.csv`):** Compresses down to **~62%** of original size.
> *Note: Already-compressed formats (PNG, JPG, PDF, ZIP) will yield minimal savings due to high baseline entropy.*

---

## 🧠 How it Works

1. **Frequency Analysis:** Scans the input stream to calculate exact byte occurrences.
2. **Tree Generation:** Builds a binary tree assigning shorter bit-sequences to higher-frequency bytes using a priority queue.
3. **Serialization:** Packs the compressed bitstream alongside metadata required for exact bit-for-bit reconstruction during decoding.

---

## 👨‍💻 Author

**Jagdish**

Electrical Engineering Undergraduate  
IIT Gandhinagar

⭐ If you found this project useful, consider giving it a star on GitHub!