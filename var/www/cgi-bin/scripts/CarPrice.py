import csv
import os
import sys

min_price_str = os.environ['MINPRICE']
max_price_str = os.environ['MAXPRICE']
current_dir = os.environ['PWD']

min_price = int(min_price_str)
max_price = int(max_price_str)

path = current_dir + "var/www/cgi-bin/database/CarPrice.csv"
output_file = "var/www/html/car_proposal.html"

if not os.path.exists(path) or not os.access(path, os.R_OK):
    print("Error: file not found or not readable", file=sys.stderr)
    exit(1)

fields = []
rows = []

with open(path, 'r') as csvfile:
    csvreader = csv.reader(csvfile)
    fields = next(csvreader)

    for row in csvreader:
        rows.append(row)

price_index = fields.index("Selling_Price")
brand_index = fields.index("Brand")
model_index = fields.index("Model")
km_index    = fields.index("KM_Driven")

filtered_rows = []

for row in rows:
    try:
        price = int(row[price_index])
        if min_price <= price <= max_price:
            filtered_rows.append(row)
    except ValueError:
        continue

with open(output_file, 'w', encoding='utf-8') as f:
    f.write("""
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Voitures disponibles</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            background-color: #f4f4f4;
            text-align: center;
        }
        h1 {
            color: #333;
        }
        table {
            width: 80%;
            margin: 20px auto;
            border-collapse: collapse;
            background: white;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        th, td {
            padding: 12px;
            border: 1px solid #ddd;
            text-align: left;
        }
        th {
            background: #007BFF;
            color: white;
        }
        tr:nth-child(even) {
            background: #f2f2f2;
        }
    </style>
</head>
<body>
    <h1>Liste des voitures disponibles</h1>
""")

    if filtered_rows:
        f.write("""
    <table>
        <tr>
            <th>Marque</th>
            <th>Modèle</th>
            <th>Kilométrage</th>
            <th>Prix (€)</th>
        </tr>
    """)

        for row in filtered_rows:
            f.write(f"""
        <tr>
            <td>{row[brand_index]}</td>
            <td>{row[model_index]}</td>
            <td>{row[km_index]} km</td>
            <td>{row[price_index]} €</td>
        </tr>
        """)

        f.write("</table>")
    else:
        f.write("<h2>Aucune voiture trouvée dans cette gamme de prix.</h2>")

    f.write("""
</body>
</html>
""")