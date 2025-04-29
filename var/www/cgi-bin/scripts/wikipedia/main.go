package main

import (
	"encoding/json"
	"fmt"
	"html/template"
	"io/ioutil"
	"net/http"
	"os"

	"github.com/skip2/go-qrcode"
)

// Structure pour parser la réponse JSON de Wikipédia
type WikiResponse struct {
	Query struct {
		Random []struct {
			Title string `json:"title"`
		} `json:"random"`
	} `json:"query"`
}

// Fonction pour obtenir une page Wikipédia aléatoire
func getRandomWikipediaURL() (string, error) {
	apiURL := "https://en.wikipedia.org/w/api.php?action=query&format=json&list=random&rnnamespace=0&rnlimit=1"

	resp, err := http.Get(apiURL)
	if err != nil {
		return "", err
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	var wikiData WikiResponse
	err = json.Unmarshal(body, &wikiData)
	if err != nil {
		return "", err
	}

	if len(wikiData.Query.Random) > 0 {
		title := wikiData.Query.Random[0].Title
		url := fmt.Sprintf("https://en.wikipedia.org/wiki/%s", title)
		return url, nil
	}

	return "", fmt.Errorf("Aucune page trouvée")
}

// Fonction pour générer un QR Code et l'enregistrer en PNG
func generateQRCode(url string, filename string) error {
	err := qrcode.WriteFile(url, qrcode.Medium, 256, filename)
	if err != nil {
		return err
	}
	return nil
}

// Fonction pour générer la page HTML avec le QR Code
func generateHTMLPage(qrImage string, wikiURL string) error {
	htmlTemplate := `<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>QR Code - Wikipédia Aléatoire</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #6e8efb, #a777e3);
            color: white;
            text-align: center;
        }
        .container {
            max-width: 600px;
            margin: 50px auto;
            background: rgba(255, 255, 255, 0.2);
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.2);
        }
        h1 {
            font-size: 24px;
        }
        img {
            margin-top: 20px;
            width: 256px;
            height: 256px;
        }
        a {
            display: inline-block;
            margin-top: 15px;
            padding: 10px 20px;
            background: #fff;
            color: #333;
            text-decoration: none;
            border-radius: 5px;
            font-weight: bold;
            transition: 0.3s;
        }
        a:hover {
            background: #ddd;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🔗 Scannez ce QR Code pour une page Wikipédia aléatoire :</h1>
        <img src="{{.QRCode}}" alt="QR Code Wikipédia">
        <p>Ou cliquez ici :</p>
        <a href="{{.WikiURL}}" target="_blank">🔍 Voir la page Wikipédia</a>
    </div>
</body>
</html>`

	// Création du fichier HTML
	file, err := os.Create(os.Getenv("QRHTML"))
	if err != nil {
		return err
	}
	defer file.Close()

	// Exécution du template HTML avec les valeurs dynamiques
	tmpl, err := template.New("webpage").Parse(htmlTemplate)
	if err != nil {
		return err
	}

	data := struct {
		QRCode string
		WikiURL string
	}{
		QRCode: qrImage,
		WikiURL: wikiURL,
	}

	return tmpl.Execute(file, data)
}

func main() {
	
	fmt.Println("🔄 Récupération d'une page Wikipédia aléatoire...")
	wikiURL, err := getRandomWikipediaURL()
	if err != nil {
		fmt.Println("❌ Erreur :", err)
		return
	}

	fmt.Println("🌐 Page trouvée :", wikiURL)

	// Nom du fichier QR Code
	qrFilename := os.Getenv("QRPATH")

	fmt.Println("📸 Génération du QR Code...")
	err = generateQRCode(wikiURL, qrFilename)
	if err != nil {
		fmt.Println("❌ Erreur lors de la génération du QR Code :", err)
		return
	}

	fmt.Println("✅ QR Code généré :", qrFilename)

	// Générer la page HTML
	fmt.Println("🖥 Génération de la page HTML...")
	err = generateHTMLPage("wikipedia_qr.png", wikiURL)
	if err != nil {
		fmt.Println("❌ Erreur lors de la création de la page HTML :", err)
		return
	}

	fmt.Println("✅ Page HTML générée : wikipedia_qr.html")
}
