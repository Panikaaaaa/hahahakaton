const API_BASE_URL = __DEV__ 
  ? Platform.OS === 'android' 
    ? 'http://10.0.2.2:8080'  // Android emulator
    : 'http://localhost:8080'  // iOS simulator
  : 'https://your-production-url.com';  // Production

export class QuizAPI {
  static async getQuestion(level) {
    try {
      const response = await fetch(`${API_BASE_URL}/level/${level}`);
      return await response.json();
    } catch (error) {
      console.error('API Error:', error);
      throw error;
    }
  }

  static async checkAnswer(level, answerIndex) {
    try {
      const response = await fetch(`${API_BASE_URL}/answer`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          level: level,
          answer: answerIndex
        }),
      });
      return await response.json();
    } catch (error) {
      console.error('API Error:', error);
      throw error;
    }
  }
}