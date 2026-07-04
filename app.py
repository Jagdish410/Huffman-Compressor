import os
import subprocess

from flask import (
    Flask,
    render_template,
    request,
    send_file,
    flash,
    redirect,
    session
)

from werkzeug.utils import secure_filename

app = Flask(__name__)
app.secret_key = "super_secret_huffman_key"

UPLOAD_FOLDER = "uploads"
OUTPUT_FOLDER = "outputs"

ALLOWED_EXTENSIONS = {
    "txt",
    "pdf",
    "docx",
    "png",
    "jpg",
    "csv",
    "bin",
    "huff"
}

app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER
app.config["OUTPUT_FOLDER"] = OUTPUT_FOLDER

os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(OUTPUT_FOLDER, exist_ok=True)


def allowed_file(filename):
    return (
        "." in filename
        and filename.rsplit(".", 1)[1].lower() in ALLOWED_EXTENSIONS
    )


@app.route("/", methods=["GET", "POST"])
def index():

    if request.method == "POST":

        if "file" not in request.files:
            flash("No file selected.")
            return redirect(request.url)

        uploaded_file = request.files["file"]
        mode = request.form.get("mode")

        if uploaded_file.filename == "":
            flash("No file selected.")
            return redirect(request.url)

        if not allowed_file(uploaded_file.filename):
            flash("Unsupported file type.")
            return redirect(request.url)

        if mode == "decompress" and not uploaded_file.filename.lower().endswith(".huff"):
            flash("Please upload only a .huff file for decompression.")
            return redirect(request.url)

        filename = secure_filename(uploaded_file.filename)

        input_path = os.path.join(
            app.config["UPLOAD_FOLDER"],
            filename
        )

        uploaded_file.save(input_path)

        if mode == "compress":
            output_filename = filename + ".huff"
        else:
            if filename.lower().endswith(".huff"):
                output_filename = filename[:-5]
            elif ".pdf" in filename.lower():
                output_filename = "restored_document.pdf"
            elif ".txt" in filename.lower():
                output_filename = "restored_document.txt"
            else:
                output_filename = "decoded_" + filename

        output_path = os.path.join(
            app.config["OUTPUT_FOLDER"],
            output_filename
        )

        cpp_executable = "./huffman.exe"

        operation = "compress" if mode == "compress" else "decompress"

        try:

            subprocess.run(
                [
                    cpp_executable,
                    operation,
                    input_path,
                    output_path
                ],
                check=True,
                capture_output=True,
                text=True
            )

            if not os.path.exists(output_path):
                flash("Processing failed. Please check that the uploaded file is valid.")
                return redirect(request.url)

            original_size = os.path.getsize(input_path)
            processed_size = os.path.getsize(output_path)

            efficiency = (
                (processed_size / original_size) * 100
                if original_size > 0
                else 100
            )

            session["success"] = True
            session["mode"] = mode
            session["original_size"] = original_size
            session["processed_size"] = processed_size
            session["efficiency"] = round(efficiency, 2)
            session["download_file"] = output_filename

            return redirect(request.url)

        except subprocess.CalledProcessError as e:
            message = e.stderr.strip() if e.stderr else "Unknown error occurred."
            flash("Processing failed: " + message)
            return redirect(request.url)

        except Exception as e:
            flash(str(e))
            return redirect(request.url)

    success = session.pop("success", False)
    mode = session.pop("mode", None)
    original_size = session.pop("original_size", None)
    processed_size = session.pop("processed_size", None)
    efficiency = session.pop("efficiency", None)
    download_file = session.pop("download_file", None)

    return render_template(
    "index.html",
    success=success,
    mode=mode,
    original_size=original_size,
    processed_size=processed_size,
    efficiency=efficiency,
    download_file=download_file
)


@app.route("/download/<filename>")
def download(filename):

    file_path = os.path.join(
        OUTPUT_FOLDER,
        filename
    )

    if not os.path.exists(file_path):
        return "File not found.", 404

    response = send_file(
        file_path,
        as_attachment=True,
        download_name=filename
    )

    @response.call_on_close
    def cleanup():

        try:

            if os.path.exists(file_path):
                os.remove(file_path)

            if filename.endswith(".huff"):
                upload_name = filename[:-5]
            else:
                upload_name = filename + ".huff"

            upload_path = os.path.join(
                UPLOAD_FOLDER,
                upload_name
            )

            if os.path.exists(upload_path):
                os.remove(upload_path)

            exact_upload = os.path.join(
                UPLOAD_FOLDER,
                filename
            )

            if os.path.exists(exact_upload):
                os.remove(exact_upload)

        except Exception as e:
            app.logger.error(str(e))

    return response


if __name__ == "__main__":
    app.run(
        debug=True,
        host="0.0.0.0",
        port=5000
    )