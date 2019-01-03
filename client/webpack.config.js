const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin')
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
  output: {
    path: outputDir,
    //publicPath: outputDir,
    filename: 'index.js',
  },
  plugins: [
    new HtmlWebpackPlugin({
      title: 'Title from the template',
      template: 'src/index.html',
    })
  ],
  devServer: {
    compress: true,
    port: 3000,
    proxy: {
      '/api': 'http://localhost:5000'
    },
    historyApiFallback: true
  },
};
