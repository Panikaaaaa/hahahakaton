import React from 'react';
import { SafeAreaView, StyleSheet } from 'react-native';
import GameMap from './src/components/GameMap';

const App = () => {
  return (
    <SafeAreaView style={styles.container}>
      <GameMap />
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f0f0f0',
  },
});

export default App;