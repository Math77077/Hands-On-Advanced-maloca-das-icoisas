import numpy as np
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
import joblib

# Function to simulate labeled RGB data
def generate_data(samples=1000):
    X = []
    y = []
    for _ in range(samples):
        # Simulate realistic RGB ranges
        r = np.random.randint(50, 200)
        g = np.random.randint(40, 180)
        b = np.random.randint(20, 150)

        # Rule to simulate jaundice cases (high blue, low red-green)
        if r < 100 and g < 100 and b > 90:
            label = 1  # jaundiced
        else:
            label = 0  # normal

        # Normalize RGB to [0, 1]
        X.append([r / 255.0, g / 255.0, b / 255.0])
        y.append(label)

    return np.array(X), np.array(y)

# Generate dataset
X, y = generate_data(1000)

# Split to verify accuracy (optional)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train KNN
knn = KNeighborsClassifier(n_neighbors=3)
knn.fit(X_train, y_train)

# Evaluate
y_pred = knn.predict(X_test)
print(classification_report(y_test, y_pred))

# Save model
joblib.dump(knn, "knn_jaundice_model.pkl")
print("✅ Model trained and saved as knn_jaundice_model.pkl")
