window.Module = window.Module || {};

window.Module.onRuntimeInitialized = () => {
  const Creact = Module._initCreact();

  let currentWidth = window.innerWidth;
  let currentHeight = window.innerHeight;

  const raylibCanvas = document.createElement("canvas");
  raylibCanvas.id = "raylib-canvas";
  raylibCanvas.style.display = "none";
  document.body.appendChild(raylibCanvas);
  Module.canvas = raylibCanvas;

  Module._init_headless(currentWidth, currentHeight, Creact);
  const displayCanvas = document.createElement("canvas");
  displayCanvas.id = "display-canvas";
  displayCanvas.width = currentWidth;
  displayCanvas.height = currentHeight;
  document.body.appendChild(displayCanvas);
  const ctx = displayCanvas.getContext("2d");

  // temporary canvas to flip the image
  const srcCanvas = document.createElement("canvas");
  srcCanvas.width = currentWidth;
  srcCanvas.height = currentHeight;
  const srcCtx = srcCanvas.getContext("2d");
  let imageData = srcCtx.createImageData(currentWidth, currentHeight);

  let isMouseDown = 0;
  let framesToRender = 2;

  // resize
  window.addEventListener("resize", () => {
    currentWidth = window.innerWidth;
    currentHeight = window.innerHeight;

    displayCanvas.width = currentWidth;
    displayCanvas.height = currentHeight;
    srcCanvas.width = currentWidth;
    srcCanvas.height = currentHeight;

    imageData = srcCtx.createImageData(currentWidth, currentHeight);
    Module._resize_frame(currentWidth, currentHeight, Creact);

    framesToRender = 1;
  });

  // mouse handling
  function sendMouseToWasm(e) {
    const rect = displayCanvas.getBoundingClientRect();
    const scaleX = displayCanvas.width / rect.width;
    const scaleY = displayCanvas.height / rect.height;

    const mouseX = (e.clientX - rect.left) * scaleX;
    const mouseY = (e.clientY - rect.top) * scaleY;

    Module._update_mouse(mouseX, mouseY, isMouseDown);
    framesToRender = 2;
  }

  window.addEventListener("keydown", (e) => {
    framesToRender = 2;
  });
  displayCanvas.addEventListener("mousemove", sendMouseToWasm);
  displayCanvas.addEventListener("mousedown", (e) => {
    if (e.button === 0) {
      isMouseDown = 1;
      sendMouseToWasm(e);
    }
  });
  displayCanvas.addEventListener("mouseup", (e) => {
    if (e.button === 0) {
      isMouseDown = 0;
      sendMouseToWasm(e);
    }
  });
  displayCanvas.addEventListener("mouseleave", (e) => {
    isMouseDown = 0;
    sendMouseToWasm(e);
  });

  //free memory
  window.addEventListener("beforeunload", () => {
    Module._destroyCreact(Creact);
  });

  // render loop
  function updateCanvas() {
    if (framesToRender > 0) {
      console.log("UPDATE");
      const pixelPointer = Module._render_frame(Creact);
      const bufferSize = currentWidth * currentHeight * 4;
      const pixelBuffer = new Uint8Array(
        Module.HEAPU8.buffer,
        pixelPointer,
        bufferSize,
      );

      imageData.data.set(pixelBuffer);
      srcCtx.putImageData(imageData, 0, 0);

      ctx.save();
      ctx.translate(0, currentHeight);
      ctx.scale(1, -1);
      ctx.drawImage(srcCanvas, 0, 0);
      ctx.restore();

      framesToRender--;
    }
    requestAnimationFrame(updateCanvas);
  }

  updateCanvas();
};
