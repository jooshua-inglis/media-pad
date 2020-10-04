package main

import (
	"io"
	"log"
	"runtime"
	"strings"
	"time"

	"github.com/jacobsa/go-serial/serial"
	"github.com/micmonay/keybd_event"
)

func keyboard(key int) {
	kb, err := keybd_event.NewKeyBonding()
	if err != nil {
		log.Fatal(err)
	}
	// For linux, need to wait 2 seconds
	if runtime.GOOS == "linux" {
		time.Sleep(2 * time.Second)
	}

	kb.SetKeys(key)
	err = kb.Launching()
	if err != nil {
		log.Fatal(err)
	}
}

func readLine(portPtr *io.ReadWriteCloser) {
	buf := make([]byte, 1280)
	var port = *portPtr
	var count = 0
	var n int
	var err error
	for {
		n, err = port.Read(buf)
		if err != nil {
			log.Fatal(err)
			break
		}
		count += n
		if strings.TrimSpace(string(buf[:n])) == "AudioHelper" {
			println("success")
			port.Write([]byte("con"))
			return
		}
	}
}

func commandListen(portPtr *io.ReadWriteCloser) {

	buf := make([]byte, 1280)
	var port = *portPtr
	var count = 0
	var n int
	var err error

	var command string
	for {
		n, err = port.Read(buf)
		if err != nil {
			log.Fatal(err)
			break
		}
		count += n
		command = strings.TrimSpace(string(buf[:n]))

		switch command {
		case "pausePlay":
			keyboard(keybd_event.VK_MEDIA_PLAY_PAUSE)
		case "volUp":
			keyboard(keybd_event.VK_VOLUME_UP)
		case "volDown":
			keyboard(keybd_event.VK_VOLUME_DOWN)

		case "nextTrack":
			keyboard(keybd_event.VK_MEDIA_NEXT_TRACK)

		case "prevTrack":
			keyboard(keybd_event.VK_MEDIA_PREV_TRACK)
		}
	}
}

func connectSerial() {
	options := serial.OpenOptions{
		PortName:        "COM3",
		BaudRate:        9600,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 4,
	}

	port, err := serial.Open(options)
	if err != nil {
		log.Fatalf("serial.Open: %v", err)
	}
	defer port.Close()

	readLine(&port)
	commandListen(&port)
}

func main() {
	connectSerial()
}
