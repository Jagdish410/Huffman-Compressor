import os
import subprocess
from flask import Flask, render_template, request, send_file, flash, redirect, url_for
from werkzeug.utils import secure_filename

app = Flask(__name__)
app.secret_key = 'super_secret_huffman_key'

UPLOAD_FOLDER = 'uploads'
OUTPUT_FOLDER = 'outputs'
ALLOWED_EXTENSIONS = {'txt', 'pdf', 'docx', 'png', 'jpg', 'huff', 'bin', 'csv'}

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['OUTPUT_FOLDER'] = OUTPUT_FOLDER

# Ensure processing directories exist locally
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(OUTPUT_FOLDER, exist_ok=True)

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file part found')
            return redirect(request.url)
            
        uploaded_file = request.files['file']
        mode = request.form.get('mode') # 'compress' or 'decompress'
        
        if uploaded_file.filename == '':
            flash('No file selected')
            return redirect(request.url)
            
        if uploaded_file and allowed_file(uploaded_file.filename):
            filename = secure_filename(uploaded_file.filename)
            input_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            uploaded_file.save(input_path)
            
            # --- Robust Filename Reconstruction Engine ---
            if mode == 'compress':
                output_filename = filename + ".huff"
            else:
                # If it cleanly ends with .huff, drop it to restore original extension
                if filename.lower().endswith('.huff'):
                    output_filename = filename[:-5]
                # Fallback safeguard in case extension was corrupted or masked by brackets
                elif ".pdf" in filename.lower():
                    output_filename = "restored_document.pdf"
                elif ".txt" in filename.lower():
                    output_filename = "restored_document.txt"
                else:
                    output_filename = "decoded_" + filename

            output_path = os.path.join(app.config['OUTPUT_FOLDER'], output_filename)
            
            # Point to your executable path (Change to "./huffman.out" if hosting on Linux cloud)
            cpp_executable = "./huffman.exe" 
            
            # Execute bit-level structural processing via external core process mapping
            # Argument flags: -c for compression, -d for decompression
            flag = "-c" if mode == 'compress' else "-d"
            
            try:
                result = subprocess.run(
                    [cpp_executable, flag, input_path, output_path],
                    capture_output=True,
                    text=True,
                    check=True
                )
                
                # Read internal metric sizing constraints safely
                original_size = os.path.getsize(input_path)
                processed_size = os.path.getsize(output_path)
                
                # Avoid dividing by zero scenario
                efficiency = (processed_size / original_size) * 100 if original_size > 0 else 100
                
                return render_template(
                    'index.html',
                    success=True,
                    mode=mode,
                    original_size=original_size,
                    processed_size=processed_size,
                    efficiency=round(efficiency, 2),
                    download_file=output_filename
                )
                
            except subprocess.CalledProcessError as e:
                flash(f"Core Engine Error: {e.stderr}")
                return redirect(request.url)
            except Exception as e:
                flash(f"System Error: {str(e)}")
                return redirect(request.url)
                
    return render_template('index.html', success=False)


@app.route('/download/<filename>')
def download(filename):
    file_path = os.path.join(OUTPUT_FOLDER, filename)
    
    if not os.path.exists(file_path):
        return "File not found.", 404

    # 📥 Forces automatic system download dialogue preventing raw in-browser text rendering
    response = send_file(
        file_path, 
        as_attachment=True, 
        download_name=filename
    )
    
    # 🧹 Automatic Storage Self-Cleanup System Callback Loop
    @response.call_on_close
    def cleanup_temporary_files():
        try:
            # Drop structural download block out of local output directories
            if os.path.exists(file_path):
                os.remove(file_path)
                
            # Mirror strip file dependencies to flush the original uploaded track out of uploads/
            if filename.endswith('.huff'):
                orig_upload = filename[:-5]
            else:
                orig_upload = filename + ".huff"
                
            upload_path = os.path.join(UPLOAD_FOLDER, orig_upload)
            if os.path.exists(upload_path):
                os.remove(upload_path)
                
            # Extra safety fallback step to clean exact matches
            exact_upload = os.path.join(UPLOAD_FOLDER, filename)
            if os.path.exists(exact_upload):
                os.remove(exact_upload)
                
            print(f"🔄 Cleaned temporary processing memory footprint for: {filename}")
        except Exception as err:
            app.logger.error(f"Storage clearance error tracking hook failed: {err}")
            
    return response

if __name__ == '__main__':
    # host='0.0.0.0' exposes local server ports over your Wi-Fi router network links cleanly
    app.run(debug=True, host='0.0.0.0', port=5000)