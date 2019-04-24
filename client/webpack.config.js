const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin')
const CopyPlugin = require('copy-webpack-plugin')
const isProd = process.env.NODE_ENV === 'production';
let outputDir = "";

if (isProd) {
  outputDir = path.join(__dirname, "../dist/public/");
} else {
  outputDir = path.join(__dirname, "build/");
}

module.exports = {
  module: {
    rules: [
      {
        test: /\.css$/,
        use: ['style-loader', 'css-loader']
      }
    ]
  },
  entry: './src/index.bs.js',
  mode: isProd ? 'production' : 'development',
  devtool: isProd ? 'source-map' : 'inline-source-map',
  output: {
    path: outputDir,
    filename: 'index.js',
  },
  plugins: [
    new HtmlWebpackPlugin({
      title: 'Title from the template',
      template: 'src/index.html',
    }),
    new CopyPlugin([
      { from: './public/service-worker.js', to: '.' },
      { from: './public/manifest.json', to: '.' },
      { from: './public/images', to: './images' },
    ])
  ],
  devServer: {
    compress: true,
    port: 3000,
    writeToDisk: true,
    proxy: {
      '/api': 'http://localhost:5000'
    },
    historyApiFallback: true
  },
};
